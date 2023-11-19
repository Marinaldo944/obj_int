# obj_int
Este trabalho final da disciplina Objetos Inteligentes Conectados do curso de Tecnologia em Análise e Desenvolvimento de Sistema da Universidade Presbiteriana Mackenzie tem por objetivo apresentar um sistema de gestão de irrigação com múltiplas fontes de água, mediante o uso de dispositivos inteligentes conectados à internet, de forma a necessitar de nenhuma, ou pouca, intervenção humana.
O projeto foi desenvolvido utilizadon-se a plataforma ESP32 programada utilizando-se a plataforma IDE Arduíno, sensores de temperatura e umidade do ar, sensor de umidade do solo, módulo ultrassônico, módulo de relê e bombas elétricas.
Os dispositivos foram separados em duas classes:
- O gerenciador: responsável por analisar a umidade do solo, temperatura e umidade do ar, coletar o nível das fontes de água e o status delas, coordenar os módulos clientes, acionando-os quando necessária a irrigação do solo;
- Os clientes: responsáveis por gerenciar cada fonte de água disponível e reportar ao módulo gerenciador o nível de água a a situação da fonte, se está disponível para uso ou não e ligar e desligar as bombas de irrigação sob o comando do módulo gerenciador.
Para a conexão com a internet foi utilizado o protocolo MQTT com o uso do servidor (broker) Mosquitto para troca de informações entre os módulos.
