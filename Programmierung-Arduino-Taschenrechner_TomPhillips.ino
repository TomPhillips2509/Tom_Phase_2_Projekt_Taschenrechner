bool parseExpression(const char* input, int &a, char &op, int &b)  {    // Der Rechenausdruck wird in die Form "a op b" aufgeteilt.
  return sscanf(input, "%d %c %d", &a, &op, &b) == 3;    // Es werden zwei Ganzzahlen und ein Oprator aus dem String erkannt -> "true" 
}

bool calculate(int a, char op, int b , int &result) {    //Durchfhrung der Berechnung
 
  switch (op) {

    case '+' : 
      result = a + b;
       return true;

    case '-': 
      result = a - b; 
      return true;
    
    case '*':
      result = a * b; 
      return true;
    
    case '/':
      if (b == 0) return false;    // Es darf nicht durch 0 geteilt werden(Mathematische Regel)
      result = a / b; 
      return true;
    default: 
      return false;
  }
}
  
void setup() {
  Serial.begin(9600);   //Schnittstelle wird initialisiert
}
void loop() {
  if (Serial.available()) {

    String input = Serial.readStringUntil('\n');

    int a, b, result;
    char op;

    if (!parseExpression(input.c_str(), a, op, b)) {
      Serial.println("Fehler: Ungültiger Ausdruck");
      return;
    }


    if (!calculate(a, op, b, result)) {
      Serial.println("Fehler: Rechenfehler");
      return;
    }
    Serial.print("Ergebnis: ");
    Serial.println(result);
  }
}