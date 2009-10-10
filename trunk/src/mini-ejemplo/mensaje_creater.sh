#!bin/sh
NOMBRE="mensaje"
for i in `seq 1 10`;
	do echo "Mensaja $i" > "mesaje"$i".txt"
done

#creamos las fotos
for i in `seq 1 10`;
do
	cp "base.jpg" "foto"$i".jpg"
done    
	