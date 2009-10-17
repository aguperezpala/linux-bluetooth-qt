#include "net.h"
#include "compiler.h"
#include "publish/sdp.h"

#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci_lib.h>
#include <sys/socket.h>

struct bluetooth_args {
	void* session_data;
	bdaddr_t device;
	uint8_t channel;
};

/*@null@*/
static
obex_t* _bluetooth_init (
	struct bluetooth_args* args,
	obex_event_t eventcb
)
{
	obex_t* handle = OBEX_Init(OBEX_TRANS_BLUETOOTH,eventcb,OBEX_FL_KEEPSERVER);
	char device[18];
  
	if (!handle)
		return NULL;

	if (BtOBEX_ServerRegister(handle, &args->device, args->channel) == -1) {
		perror("BtOBEX_ServerRegister");
		return NULL;
	}
	(void)ba2str(&args->device, device);
	fprintf(stderr, "Listening on bluetooth/[%s]:%u\n", device, (unsigned int)args->channel);

	args->session_data = bt_sdp_session_open(&args->device, args->channel);
	if (!args->session_data) {
		fprintf(stderr, "SDP session setup failed, disabling bluetooth\n");
		OBEX_Cleanup(handle);
		return NULL;
	}
	return handle;
}

static
obex_t* bluetooth_init(
	void* arg,
	obex_event_t eventcb
)
{
	return _bluetooth_init((struct bluetooth_args*)arg, eventcb);
}

static
void _bluetooth_cleanup(
	struct bluetooth_args *args,
	obex_t __unused *ptr
)
{
	if (args->session_data) {
		bt_sdp_session_close(args->session_data, &args->device);
		args->session_data = NULL;
	}
}

static
void bluetooth_cleanup(
	void* arg,
	obex_t* ptr
)
{
	_bluetooth_cleanup((struct bluetooth_args*)arg, ptr);
}

static
int bluetooth_security_init(
	void __unused *arg,
	obex_t *ptr
)
{
	int sock = OBEX_GetFD(ptr);
	int err = 0;
	const uint32_t options = (RFCOMM_LM_AUTH | RFCOMM_LM_SECURE);
	uint32_t optval = 0;
	socklen_t optlen = sizeof(optval);

	if (sock < 0)
		return -ENOTSOCK;

	err = getsockopt(sock, SOL_RFCOMM, RFCOMM_LM, &optval, &optlen);
	if (err < 0) {
		perror("Getting RFCOMM_LM");
		return -errno;
	}
	if (optlen != sizeof(optval))
		return -EINVAL;

	if ((optval & options) != options) {
		optval |= options;
		err = setsockopt(sock, SOL_RFCOMM, RFCOMM_LM, &optval, optlen);
		if (err < 0) {
			perror("Setting RFCOMM_LM");
			return -errno;
		}
	}
	return 0;
}

static
int bluetooth_get_peer(
	obex_t* handle,
	char* buffer,
	size_t bufsiz
)
{
	struct sockaddr_rc addr;
	socklen_t addrlen = sizeof(addr);
	char addrstr[128];
	char tmp[256];

	int status;
	int sock = OBEX_GetFD(handle);

	if (sock == -1)
		return -EBADF;
	status = getpeername(sock, (struct sockaddr*) &addr, &addrlen);
	if (status == -1)
		return -errno;
	if (addr.rc_family != AF_BLUETOOTH)
		return -EBADF;

	memset(addrstr, 0, sizeof(addrstr));
	ba2str(&addr.rc_bdaddr, addrstr);
	status = snprintf(tmp, sizeof(tmp), "bluetooth/[%s]:%u", addrstr, addr.rc_channel);

	if (buffer)
		strncpy(buffer, tmp, bufsiz);

	return status;
}

static
struct net_funcs bluetooth_funcs = {
	.init = bluetooth_init,
	.cleanup = bluetooth_cleanup,
	.get_peer = bluetooth_get_peer,
	.security_init = bluetooth_security_init
};

int bluetooth_setup(
	struct net_data* data,
	char* device,
	uint8_t channel
)
{
	struct bluetooth_args* args = malloc(sizeof(*args));
	int hciId  = -1;
	data->arg = args;
	if (!args)
		return -errno;

	if (device) {
		if (strlen(device) == 17) /* 11:22:33:44:55:66 */
			hciId = hci_devid(device);
		else if (1 != sscanf(device, "hci%d", &hciId))
			return -1;
	}

	if (hciId >= 0)
		hci_devba(hciId, &args->device);
	else
		bacpy(&args->device, BDADDR_ANY);

	args->channel = channel;
	data->funcs = &bluetooth_funcs;
	return 0;
}
