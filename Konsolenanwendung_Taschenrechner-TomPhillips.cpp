#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>

int main() {
    // Konfiguration Schnittstelle (Port & Baudrate werden festgelegt)
    const char* portName = "\\\\.\\COM4";  
    const int baudRate = 9600;

    // Serielle Schnittstelle initiallisieren
    HANDLE hSerial = CreateFile(    // serielle Schnittstelle wird geöffnet
        portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    // Wenn der konfigurierte COM-Port nicht ereicht wird -> Fehler wird generiert
    if (hSerial == INVALID_HANDLE_VALUE) {    //Im Fehlerfall: Port ist nicht erreichbar -> Vrbindung wird abgebrochen
        std::cerr << "Fehler beim Öffnen des Ports COM4\n";
        return 1;
    }

    std::cout << "Verbunden mit Arduino auf COM4\n";    // Statusmeldung wird ausgegeben 

    // Serielle Parameter erstellt
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    // Parameter werden in die Struktur übertragen
    GetCommState(hSerial, &dcbSerialParams);
    // Baudrate, Bitgröße, Stoppbits, Parität wird gesetzt (Einstellung für Standard-Arduino)
    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    // Timeout konfigurieren (Wartezeit auf Arduino)
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 2000;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    // Logdatei wird geöffnet
    std::ofstream log("log.txt", std::ios::app);

    // Benutzeranweisung wird in Konsole ausgegeben
    std::cout << "Herzlich Wilkommen im Taschenrechner von Tom Phillips. Bitte beachten Sie die Anweisungen!\n";    //Begrüßung    
    std::cout << "Rechnung eingeben (z.B. 3 + 4)\n\n";    //Operanden sollen vom Benutzer jetzt eingegeben werden
    std::cout << "Bitte beenden Sie ihren Ausdruck mit 'goArduino'\n\n\n";    //Mit "goArduino" wird der Ausdruck an den Arduino gesendet

    //Warteschleife für Eingabe vom Benutzer
    while (true) {
       
        // Eingabe wird gelesen
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "go")    //Abbruchbedingung (Die Schleife endet)
            break;

        // Zeilenumbruch wird angehangen
        input += "\n";

        // Senden an Arduino
        DWORD bytesWritten;
        WriteFile(hSerial, input.c_str(), input.size(), &bytesWritten, NULL);

        // Empfangen vom Arduino
        char buffer[256];
        DWORD bytesRead;
        ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        buffer[bytesRead] = '\0';

        std::cout << "Arduino: " << buffer;    //Antwort wird angezeigt

        // Logging in Datei
        log << "PC: " << input;
        log << "Arduino: " << buffer << "\n";
    }
    //Port und Datein werden geschlossen
    CloseHandle(hSerial);
    log.close();

    std::cout << "Verbindung geschlossen.\n";
    return 0;
}

