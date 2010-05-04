#!/bin/sh

# shut_bluetoothd() {
# 	echo "Dando de baja bluetoothd"
# 	if [ `killall bluetoothd` ]; then
# 		echo "Listo"
# 	else
# 		echo "algun error"
# 	fi
# }
# 
# shut_hci() {
# 	echo "dando de baja la interface"
# 	if [ `hciconfig hci0 down` ]; then
# 		echo "listo"
# 	else
# 		echo "no se pudo dar de baja."
# 	fi
# }
# 
# shut_bluetooth_service() {
# 	echo "dando de baja al servicio de bluetooth"
# 	if [ `/etc/init.d/bluetooth stop` ]; then
# 		echo "listo"
# 	else
# 		echo "Error."
# 		
# 	fi
# }
# 
# shut_btusb() {
# 	echo "dando de baja al modulo btusb"
# 	if [ `rmmod btusb` ]; then
# 		echo "listo"
# 	else
# 		echo "Error."
# 		
# 	fi
# }
# 
# shut_reloading_all() {
# 	echo "Cargando TODO"
# 	if `modprobe btusb && hciconfig hci0 up && /etc/init.d/bluetooth start`; then
# 		echo "listo"
# 	else
# 		echo "todo mal"
# 		exit 1
# 	fi
# 	
# }
# echo "veamos que onda este script"
# shut_bluetoothd
# shut_hci
# shut_bluetooth_service
# shut_btusb
# shut_reloading_all
# echo "listo"
# exit 0

echo "matando bluetoothd"
sudo killall bluetoothd
echo "matando frenando service bluetooth"
sudo /etc/init.d/bluetooth stop
echo "matando la interface hci0"
sudo hciconfig hci0 down
echo "matando el modulo btusb"
sudo rmmod btusb
echo "un poco de info hciconfig -a"
sudo hciconfig -a
echo "cargando el btusbe"
sudo modprobe btusb
echo "info hciconfig -a"
sudo hciconfig -a
echo "matamos de nuevo por las dudas hci0"
sudo hciconfig hci0 down
echo "dando de alta hci0"
sudo hciconfig hci0 iup
echo "cargando de nuevo bluetooth service"
sudo service bluetooth start