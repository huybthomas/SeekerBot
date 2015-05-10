#Copies the helloworld project to a new location with a new name
#param 1: path to new location
#param 2: new project name


mkdir $1 || continue
cp * $1
cd $1
make clean
sed -i s/helloworld/$2/g helloworld.cbp
sed -i s/helloworld/$2/g Makefile
mv helloworld.cbp $2.cbp
rm *~ || continue
rm *.workspace || continue
rm *.layout || continue
rm newproject.sh || continue
