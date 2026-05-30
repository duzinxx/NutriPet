#include <Servo.h>

#include <LiquidCrystal_I2C.h>

const byte SERVO=6, MAIS=4, MENOS=5, SET=2, RESET=3,
TRINTAMAIS=7, TRINTAMENOS=8, LIBERAR=9, PAUSA=10;

//configurando
int minutoConfig = 0;
int horaConfig = 0;

int horaAtual = 0;
int minutoAtual = 0;
int segundoAtual = 0;

bool iniciado = false;
bool pausado = false;
bool cliquePausa = false;
String modo;


Servo motor;
LiquidCrystal_I2C tela(0x20, 16, 2);

void setup()
{
  Serial.begin(9600);
  pinMode(TRINTAMENOS, INPUT_PULLUP);
  pinMode(TRINTAMAIS, INPUT_PULLUP);
  pinMode(MAIS, INPUT_PULLUP);
  pinMode(MENOS, INPUT_PULLUP);
  pinMode(SET, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(LIBERAR, INPUT_PULLUP);
  pinMode(PAUSA, INPUT_PULLUP);
  

  motor.attach(SERVO);

  tela.init();
  tela.backlight();

  motor.write(0);
  
  tela.setCursor(0,0);
  tela.print("NutriPet");
}

void loop()
{
  delay(1000);
  
  if(digitalRead(SET)==0 && !iniciado){

   if(horaAtual == 0 && minutoAtual == 0){

      tela.clear();
      tela.setCursor(0,0);
      tela.print("Defina tempo!");
   }
   else{

      iniciado = true;

      horaConfig = horaAtual;
      minutoConfig = minutoAtual;
   }

   delay(300);
}
  
  if(iniciado && !pausado){
    //Execução do timer e verificação da hora de liberação
    if(segundoAtual == -1){
      minutoAtual--;
      segundoAtual = 59;
    }

    if(minutoAtual == -1){
      horaAtual--;
      minutoAtual = 59;
    }

    //Amostragem do tempo
    amostradinho();

    //Verificar se o timer zerou
   if(horaAtual == 0 && minutoAtual == 0 && segundoAtual == 0){

  motor.write(180);

  tela.clear();
  tela.setCursor(0,0);
  tela.print("Hora do rango!");

  Serial.println("Hora do rango!");

  delay(3000);

  motor.write(0);

  horaAtual = horaConfig;
  minutoAtual = minutoConfig;
  segundoAtual = 0;

  tela.clear();
}
    segundoAtual--; //Diminui os segundos a cada volta no loop
  	//Fim do timer
  }
  
  //Verificação do cliqueSet
  if(!iniciado) {
    horaMais();
    horaMenos();
    trintaMais();
    trintaMenos();
  }
  
  liberar();
  
  reconfigurar();
  
  pausar();
}

/////////////////////////////////////
void amostradinho(){
  Serial.print(horaAtual);
  Serial.print(":");
  Serial.print(minutoAtual);
  Serial.print(":");
  Serial.println(segundoAtual);

  //tela.clear();
  tela.setCursor(0,0);
  tela.print("                ");
  tela.setCursor(0,0);
  tela.print(horaAtual);
  tela.print(":");

  if(minutoAtual < 10) {
    tela.print("0");
    tela.print(minutoAtual);
    tela.print(":");
  } else {
    tela.print(minutoAtual);
    tela.print(":");
  }

  if(segundoAtual < 10) {
    tela.print("0");
    tela.print(segundoAtual);
  } else {
    tela.print(segundoAtual);
  }
  
  if(iniciado && !pausado){
     tela.setCursor(0,1);
          
     if(modo != "Timer rodando"){
        modo = "Timer rodando";
        tela.print(modo);
     }
  } 
  
  
  
  
  else {
     tela.setCursor(0,1);
           
     if(modo != "Configurando"){
       modo = "Configurando";
       tela.print(modo);
     }
  }
}


void horaMais() {
  if(digitalRead(MAIS)==0 ){
  	horaAtual = min(horaAtual+1, 23);
    amostradinho();
  }
}

void horaMenos() {
  if(digitalRead(MENOS)==0 ){
  	horaAtual = max(horaAtual-1, 0);
    amostradinho();
  }
}

void trintaMais() {
  if(digitalRead(TRINTAMAIS)==0 ){

    if(horaAtual == 23 && minutoAtual >= 30){
      horaAtual = 23;
      minutoAtual = 59;
    }
    else{
      minutoAtual += 30;

      if(minutoAtual >= 60){
        minutoAtual -= 60;
        horaAtual++;
      }
    }

    amostradinho();
  }
}

void trintaMenos() {
  if(digitalRead(TRINTAMENOS)==0 ){
    minutoAtual = minutoAtual - 30;
    
    if(horaAtual > 0 && minutoAtual == -30){
      horaAtual--;
      minutoAtual = 30;
    } else if(horaAtual == 0 && minutoAtual == -30) {
      minutoAtual = 0;
    }
    amostradinho();
  }
}



void ativar(){
  
  iniciado = true;
  
  horaConfig = horaAtual;
minutoConfig = minutoAtual;
  
  Serial.println("Timer iniciado!");
}


void reconfigurar(){
  if(digitalRead(RESET)==0 ){
  tela.clear();
  tela.setCursor(0,0);
  tela.print("Reiniciado!");
  Serial.print("Timer reconfigurado!");
  iniciado = false;
  horaAtual = 0;
  minutoAtual = 0;
  segundoAtual = 0;
  }
}

void liberar(){
  if(digitalRead(LIBERAR)==0){
  	if(horaAtual >= 0 && minutoAtual >= 0 && segundoAtual >= 0){

  motor.write(180);

  tela.clear();
  tela.setCursor(0,0);
  tela.print("Hora do rango!");

  Serial.println("Hora do rango!");

  delay(3000);

  motor.write(0);

  horaAtual = horaConfig;
  minutoAtual = minutoConfig;
  segundoAtual = 0;

  tela.clear();
	}
  }
}

void pausar(){
  if(digitalRead(PAUSA)==0){
    cliquePausa = true;
   pausado = !pausado;
  }
  if(digitalRead(PAUSA)==1){
  cliquePausa=false;
  }
}
