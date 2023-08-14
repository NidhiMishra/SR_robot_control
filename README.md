This is in development. Therefore the installation process is not fully automatic.

You need to create the build folder using cmake to create a visual studio 2010 C++ solution file.
Use the CMakeLists.txt provided in the repository for this purpose.

Instruction for installation:

(1)Install Matlab R2012a


(4) Open i2p_nadine_control in VC++
Open the properties and change the following:
-In General: set Common Language Runtime Support to Common Language Support (/clr)

-In Common Properties: Add New Reference, COM, select Matlab Application (Version 7.14) Type Library
-In C/C++ Code Generation change the following:
-set Enable C++ Exceptions to Yes with SEH Exceptions (/EHa)
-set Basic Checks to Default 

