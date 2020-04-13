; Installs i2c library (Q&D install script)
echo "Copy library..."
copy library/build/build-000/i2c.library.000 to LIBS:i2c.library
echo "flush libs..."
flush -l
echo "Call unittest..."
unittest/build/build-000/unittest
