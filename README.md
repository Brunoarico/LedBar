# Led Bar Project

Projeto de engenharia reversa nas barras de Led com CI de registrador de deslocamento (Shift Reg).

## Conclusão

Na barra de led constam 8 Ci's registadores de deslocamento com comando de 208 bits cada, com cada cor tendo 16 bits de resolução.
Cada CI controla 4 Leds individualmente, a barra possui 30 leds, tendo então um dos CI's o controle sobre apenas 2 Leds.
Um provável CI análogo é o MY9221.

## Definir SSID e PASSWORD do Wifi no código

Para definir o SSID e o respectivo PASSWORD abrir o arquivo que se encontra dentro do diretório *src* denominado **ssid.h** e inserir entre as respectivas aspas o nome da rede wifi e sua senha.

## Configuração de ambiente

Instalar o programa editor [Atom](https://atom.io/) e adicionar a ele o plugin [PlatformIO](https://platformio.org/install/ide?install=atom)

Abrir o Atom e clicar no botão home do PlatformIO, na sequencia abrir este projeto.

Para uploadar o firmware para a placa basta ir no menu superior, clicar em upload (certificando que a placa esta conectada ao computador) e aguardar o processo de compilação e envio do binário para a placa. Se tudo correr certo nenhuma tela de erro ficará aberta.

## Elementos do sistema

* Microcontrolador Wemos D1 (ESP8266)
* Fonte 24V
* Regulador de tensão step-down LM2596
* Barra Led

Na barra Led, próximo ao Led1 (Nunca usar os terminais pŕoximos ao Led30) conectar os pinos do wemos da seguinte forma:

|Barra| Wemos|
|----|----|
|+5V|3V3|
|GND|G|
|CLK|D3|
|DAI|D2|

Na conexão da fonte com o regulador LM2596 e fonte

|LM2596|Fonte|
|----|----|
|-IN|Negativo|
|+IN|Positivo|

Checar com um multímetro a saída de tensão nos pinos **OUT+** e **OUT-**, se diferente de 5V Ajustar no trimpot da placa até que a tensão fique o mais próxima desse valor.
Apos esta verificação conectar o regulador ao Wemos da seguinte forma

|LM2596|WEMOS|
|----|----|
|-OUT|G|
|+OUT|5V|

## Verificando a conexão com a rede e o IP da placa

Para verificar o IP da placa conecte a placa ao computador por meio de USB e no menu superior do atom, navegar em *PlatformIO \> Serial Monitor*.
Selecionar a porta equivalente a placa USB (Se não houver outro dispositivo serial conectado, aparecerá apenas uma opção se tudo correr bem) e a baudrate em *115200* e então clicar *Start*

Uma mini janela será aberta.

Nesse momento, pressione o botão de reset no wemos e veja se nessa janela vários pontos começarão a ser escritos.
Esses pontos ficarão surgindo até o momento que o wemos conseguira a conexão com o roteador. Eventualmente se ele não conseguir dentro de um minuto, cheque se o arquivo *ssid.h* está com as informações corretas.
Se tudo correr bem um numero de IP será printado nessa tela.
Este é o IP da placa nesse momento.

## Enviar UDP


Para enviar UDP para a placa, basta enviar um pacote para o IP dela, na porta **2807** com a **string** no seguinte formato:

"\<Numero_do_Led\> \<R\> \<G\> \<B\>"

Sendo:
* Número do Led: O número do led que se deseja acender
* R o valor de vermelho, sendo o mínimo 0 e o máximo 30
* G o valor de verde, sendo o mínimo 0 e o máximo 30
* B o valor de azul, sendo o mínimo 0 e o máximo 30

Exemplo:

"1 30 0 0" = Acende o led1 com o vermelho no maximo, 0 de azul e 0 de verde.
