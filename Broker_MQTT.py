import paho.mqtt.client as mqtt
import time

#Função responsável por realizar a conexão com o broker
def on_connect(client, userdata, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("outTopic")

#Função on_message, responsável por exibir os valores enviados
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload.decode("utf-8")))

client = mqtt.Client("PC")
client.on_connect = on_connect
client.on_message = on_message

client.connect("mqtt.eclipse.org", 1883, 60)
client.subscribe("Supervisorio/Inversor1/Corrente")
while True:
	client.loop_start()
	time.sleep(4)
