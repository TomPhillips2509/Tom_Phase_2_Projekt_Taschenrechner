bool parseExpression(const char* input, int &a, char &op, int &b)  {    // Der Rechenausdruck wird in die Form "a op b" aufgeteilt.
  return sscanf(input, "%d %c %d", &a, &op, &b) == 3;    // Es werden zwei Ganzzahlen und ein Oprator aus dem String erkannt -> "true" 
}

bool calculate(int a, char op, int b , int &result) {    //Durchführung der Berechnung
 
  switch (op) {    // Prüft, welche Rechenoperation folgend verwendet wird.

    case '+' : 
      result = a + b;    //Berechnung: Addition
       return true;

    case '-': 
      result = a - b;    //Berechnung: Subtration
      return true;
    
    case '*':
      result = a * b;    //Berechnung: Multiplikation
      return true;
    
    case '/':
      if (b == 0) return false;    // Es darf nicht durch 0 geteilt werden(Mathematische Regel)
      result = a / b;    //Berechnung: Division
      return true;
    default: 
      return false;
  }
}
  
void setup() {
  Serial.begin(9600);   //Schnittstelle wird initialisiert
}
void loop() {
  if (Serial.available()) {    //Wartet auf eine Eingabe über die seriellen Schnittstelle

    String input = Serial.readStringUntil('\n');    //Liest den Ausdruck, über die Konsoleneingabe, bis zum Zeilenumbruch

    int a, b, result;    // Operanten werden in einem Integer dargestellt
    char op;    //Operator wird in einem Char dargestellt

    if (!parseExpression(input.c_str(), a, op, b)) {    //Ausdruck wird zerlegt, um auf Fehler zu prüfen. Falls Fehler -> Fehler: Ungültiger Ausdruck
      Serial.println("Fehler: Ungueltiger Ausdruck");
      return;
    }


    if (!calculate(a, op, b, result)) {
      Serial.println("Fehler: Rechenfehler");    //Ausgabe Fehler: Rechenfehler
      return;
    }
    Serial.print("Ergebnis: ");    //Ausgabe von dem Ergebnis
    Serial.println(result);
  }
}