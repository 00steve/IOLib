// SerialPortTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../SerialPort/SerialPort.h"
#include "../SerialPort/TinyStream.h"
#include "../SerialPort/Bit.h"

int main(){


    //char i = 16;
    //Bit::Print(i); std::cout << "\n";
    //char x = BITMASKL2;
    //Bit::Print(x); std::cout << "\n";
    //x = x >> 4;
    //Bit::Print(x); std::cout << "\n";
    //char f = 143;
    //Bit::Print(f); std::cout << "\n";
    //f = Bit::Get(f, 5, 3);
    //Bit::Print(f); std::cout << " = " << (unsigned int)f <<"\n";
    

    SerialPort sp = SerialPort();
    TinyStream ts = TinyStream();

    ts.SetInput(&sp);

    sp.Open("COM4");

    while (true) {
        sp.ReadToBuffer();
        ts.Update();
    }
    

    //sp.Close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
