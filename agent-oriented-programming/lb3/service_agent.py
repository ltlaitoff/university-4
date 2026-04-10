import random
import asyncio

from spade.template import Template
from spade.agent import Agent
from spade.behaviour import CyclicBehaviour
from spade.message import Message

request_data_template = Template()
request_data_template.set_metadata("performative", "request")
request_data_template.set_metadata("language", "FIFA-SL")
request_data_template.set_metadata("ontology", "REQUEST-DATA")

agree_response_template = Template()
agree_response_template.set_metadata("performative", "agree")
agree_response_template.set_metadata("language", "FIFA-SL")
agree_response_template.set_metadata("ontology", "AGREE")

refuse_response_template = Template()
refuse_response_template.set_metadata("performative", "refuse")
refuse_response_template.set_metadata("language", "FIFA-SL")
refuse_response_template.set_metadata("ontology", "REFUSE")

data_response_template = Template()
data_response_template.set_metadata("performative", "inform")
data_response_template.set_metadata("language", "FIFA-SL")
data_response_template.set_metadata("ontology", "DATA")

class ServiceAgent(Agent):
    # Visual(in console) agent name. Different from jid
    name = "Unknown receiver"

    # How long we wait for message? In seconds
    message_timeout = 10

    def __init__(self, jid: str, password: str, name: str):
        super().__init__(jid, password)
        self.name = name

    class ReceiveBehaviour(CyclicBehaviour):
        async def send_data(self, to_id, value):
            random_time = random.randint(0, 100) / 50

            print(f"[ASYNC] {self.agent.name}: Schedule send message to {to_id} with value {value} after {random_time} seconds")
            await asyncio.sleep(random_time)

            message = Message(to=to_id)
            message.body = str(value)
            message.set_metadata("language", "FIFA-SL")
            message.set_metadata("performative", "inform")
            message.set_metadata("ontology", "DATA")

            assert data_response_template.match(message), "Data message should match data template"
            await self.send(message)
            print(f"[ASYNC] {self.agent.name}: Send data to {to_id}. Value is {value}")

        async def run(self):
            print(f"\n{self.agent.name}: Waiting for messages!")
            received_message = await self.receive(timeout=self.agent.message_timeout)

            if not received_message:
                print(f"{self.agent.name}: Bye!")
                await self.agent.stop()
                return

            assert request_data_template.match(received_message), "Service agent receive only request data messages"
            print(f"{self.agent.name}: Message from \"{received_message.sender}\" received!")

            random_value = random.randint(0, 10)
            message = Message(to=str(received_message.sender))
            message.body = "Request response"
            message.set_metadata("language", "FIFA-SL")

            if random_value > 8:
                message.set_metadata("performative", "refuse")
                message.set_metadata("ontology", "REFUSE")
                assert refuse_response_template.match(message), "Refuse message should match refuse template"
                await self.send(message)
                print(f"{self.agent.name}: Send refuse message to {message.to}")
                return

            message.set_metadata("performative", "agree")
            message.set_metadata("ontology", "AGREE")
            assert agree_response_template.match(message), "Agree message should match agree template"
            await self.send(message)
            print(f"{self.agent.name}: Send agree message to {message.to}")

            asyncio.create_task(self.send_data(str(received_message.sender), random_value))

    async def setup(self):
        print(f"{self.name}: Hi!")

        self.add_behaviour(self.ReceiveBehaviour())
