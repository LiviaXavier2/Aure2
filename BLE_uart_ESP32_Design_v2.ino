#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
String txValue="";

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

/*******************************************************************************************
 * Não alterar o serviços e as características, a Unity já está configurada
 * Podem mudar o Nome, lembrando de mudar na Unity (está no Inspector do Game Object Canvas)
 ******************************************************************************************/
#define SERVICE_UUID           "0000ffe0-0000-1000-8000-00805f9b34fb" // UART service UUID
#define CHARACTERISTIC_UUID_RX "0000ffe1-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_TX "0000ffe2-0000-1000-8000-00805f9b34fb"
#define NOME "aure"//vejam sempre se esse nome está igualzinho na Unity!!!!
#define SENSOR_BAT_CARDIACOS A0//deixa tudo que é constante como maiúsculos, é mais fácil de ler


//Definição de Variáveis de controle do sensor de batimentos
float val_Sensor_Bat_Cardiaco;
float val_Sensor_Bat_Cardiaco_Bin;
int contador_Batimentos;
float batimentos_Por_minuto;
int tempo_Inicial;
int tempo_Final;
int ativa_Desativa_Medicao = 0;  //0, desativa e 1 ativa

//Definição de variáveis de controle dos leds
CRGB leds[numero_Leds_Usados];
int val_R; //Controle de intesidade do vermelho
int val_G; //Controle de intesidade do verde
int val_B; //Controle de intesidade do azul
int Recebe_Escolha_Tom;
/* Códigos Char possíveis: 1:Verde Pastel, 2: Verde Mostarda, 3: Terroso, 
4: Amarelo Fundo, 5: Verde Preto, 0: Desligado, 10: Vinho */
//Definicao variaveis de mapeamento de intesidade de frequência

/*int map_R;
int map_G;
int map_B;
int map_Nivel;*/

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
/**************************************************************************************************
   Aqui está a função que deve ler os batimentos cardiacos
**************************************************************************************************/
 void Batimentos() {
    //Aqui vocês dão uma olhad para ver quais são os valores
    //para calcular corretamente os batimentos/segundo
    val_Sensor_Bat_Cardiaco = analogRead(SENSOR_BAT_CARDIACOS);     
    val_Sensor_Bat_Cardiaco_Bin = (val_Sensor_Bat_Cardiaco * 3.3) / 4095;
    if (val_Sensor_Bat_Cardiaco_Bin > 1.5)
    {
      contador_Batimentos++;
    }
    //Liguem o Serial Plotter no menu Tools para ver a forma
    //de onda.
    Serial.println(val_Sensor_Bat_Cardiaco);
    delay(200);
}

  
/***************************************************************
* Aqui vc coloca a programação da placa para o recebimento
* de mensagens vindas do aplicativo (Do android ou do Windows)
***************************************************************/
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) 
    {
      std::string rxValue = pCharacteristic->getValue();
      //So utilizei os valores que vocês definiram
      //1 liga
      //0 desliga
      if(rxValue=="1")
      {
        Serial.println("LIGOU");
        ativa_Desativa_Medicao=1;
      }
      else if(rxValue=="0")
      {
        Serial.println("DESLIGOU");
        ativa_Desativa_Medicao=0;
      }
      else {
        Recebe_Escolha_Tom = rxValue;
      }
    }
};


void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init(NOME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
/**************************************************************************
 *   Dentro deste if abaixo, você coloca a 
 *   lógica de envio de dados;
 *************************************************************************/
    if (deviceConnected) {

          controle_LED();

        if(ativa_Desativa_Medicao == 1)
        {
        //Só gerando conteúdo para poder enviar
        //
          Batimentos();
          txValue="";
          txValue+=String(val_Sensor_Bat_Cardiaco)+"\n";
          txValue+=String(val_Sensor_Bat_Cardiaco_Bin)+"\n";
          txValue+=String(contador_Batimentos);
        }

        uint8_t byteArray[txValue.length() + 1]; // +1 para o caractere nulo de terminação
        txValue.getBytes(byteArray, sizeof(byteArray));
        pTxCharacteristic->setValue(byteArray,sizeof(byteArray));
        pTxCharacteristic->notify();
		    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}





// Se for necessário pegar o resultado de RXValue para inteiro (parse Int)


/*Definição de variáveis de controle dos leds
CRGB leds[numero_Leds_Usados];
int val_R; //Controle de intesidade do vermelho
int val_G; //Controle de intesidade do verde
int val_B; //Controle de intesidade do azul
int Recebe_Escolha_Tom;*/
/* Códigos Char possíveis: 1:Verde Pastel, 2: Verde Mostarda, 3: Terroso, 
4: Amarelo Fundo, 5: Verde Preto, 0: Desligado, 10: Vinho */
//Definicao variaveis de mapeamento de intesidade de frequência
/*int map_R;
int map_G;
int map_B;
int map_Nivel;*/


// Lógica dos Leds

void controle_LED()
{
  switch (Recebe_Escolha_Tom)
  {
    //Cor verde Pastel
    case 2:
          {
           val_G=200 ,val_R=100 ,val_B=50;
           for(int i=0;i<=12;i++)
           {leds[i+1] = CRGB(val_G,val_R,val_B);
            FastLED.show();
            delay(1000);
            }
            for(int i=12;i>=0;i--)
                  {leds[i] = CRGB(0,0,0);
                    FastLED.show();
                    delay(1000);
                  } 
            break;
          }
    //Cor verde Mostarda
    case 3:
          {
           val_G=217 ,val_R=217 ,val_B=25;
           for(int i=0;i<=12;i++)
           {leds[i+1] = CRGB(val_G,val_R,val_B);
            FastLED.show();
            delay(1000);
            }
            for(int i=12;i>=0;i--)
                  {leds[i] = CRGB(0,0,0);
                    FastLED.show();
                    delay(1000);

                  } 
            break;
          }
     //Cor terroso    
    case 4:
          {
           val_G=20 ,val_R=135 ,val_B=3;
           for(int i=0;i<=12;i++)
           {leds[i+1] = CRGB(val_G,val_R,val_B);
            FastLED.show();
            delay(1000);
            }
            for(int i=12;i>=0;i--)
                  {leds[i] = CRGB(0,0,0);
                    FastLED.show();
                    delay(1000);

                  } 
            break;
          }      
     //Cor amarelo fundo    
    case 5:
          {
           val_G=245 ,val_R=245 ,val_B=100;
           for(int i=0;i<=12;i++)
           {leds[i+1] = CRGB(val_G,val_R,val_B);
            FastLED.show();
            delay(1000);
            }
            for(int i=12;i>=0;i--)
                  {leds[i] = CRGB(0,0,0);
                    FastLED.show();
                    delay(1000);

                  } 
            break;
          } 
    //Cor verde-preto
    case 6:
          {
           val_G=6 ,val_R=1 ,val_B=0;
           for(int i=0;i<=12;i++)
           {leds[i+1] = CRGB(val_G,val_R,val_B);
            FastLED.show();
            delay(1000);
            }
            for(int i=12;i>=0;i--)
                  {leds[i] = CRGB(0,0,0);
                    FastLED.show();
                    delay(1000);

                  } 
            break;
          }  
    default:
            {
              val_G=0 ,val_R=0 ,val_B=0;
                  for(int i=0;i<=12;i++)
                  {leds[i] = CRGB(val_G,val_R,val_B);
                    FastLED.show();

                  }
                   for(int i=12;i>=12;i--)
                  {leds[i] = CRGB(val_G,val_R,val_B);
                    FastLED.show();

                  } 
                    break;
            }         
  }
}

/*void luminosidade()
{
   map_Nivel = map (val_Sensor_Bat_Cardiaco_Bin,1.480,1.548,0,12); 
   for(int i= map_Nivel;i<=12;i++)
      {leds[i] = CRGB(val_G,val_R,val_B);
        FastLED.show();
        delay(500);
      }
   for (int i=12; i>map_Nivel;i--)
      {leds[i] = CRGB(val_G,val_R,val_B);
        FastLED.show();
        delay(500);
      }
  }*/