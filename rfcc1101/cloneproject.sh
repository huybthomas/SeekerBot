

if [ "$#" -ne 2 ]; then
	echo "Invalid syntax"
	echo "Copies the rfcc1101 project to a new location with a new name"
	echo "param 1: path to new location"
	echo "param 2: new project name"
	exit -1
fi

mkdir $1 || true
cp * $1
cd $1
make clean
sed -i s/rfcc1101/$2/g rfcc1101.cbp
sed -i s/rfcc1101/$2/g Makefile
sed -i s/rfcc1101/$2/g cloneproject.sh
mv rfcc1101.cbp $2.cbp
rm -f *~ || true
rm -f *.workspace || true
rm -f *.layout || true
echo "DONE"

