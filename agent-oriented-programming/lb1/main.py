import random

import spade
from spade.agent import Agent
from spade.behaviour import CyclicBehaviour
import asyncio
from spade.message import Message
from spade.template import Template

import keys

int_message_template = Template()
int_message_template.set_metadata("performative", "inform")
int_message_template.set_metadata("language", "FIFA-SL")
int_message_template.set_metadata("ontology", "INTEGER")

class AverageNumbersAgent(Agent):
    # Custom agent identifier. For debug purposes only
    name = "Unknown receiver"
    message_time = 5
    sum = 0
    count = 0

    def __init__(self, jid: str, password: str, name: str):
        super().__init__(jid, password)
        self.name = name

    class HandleIntBehaviour(CyclicBehaviour):
        async def run(self):
            print(f"{self.agent.name}: Waiting for messages!")
            message = await self.receive(timeout=self.agent.message_time)

            if not message:
                print(f"{self.agent.name}: Bye!")
                await self.agent.stop()
                return

            assert int_message_template.match(message)
            print(f"{self.agent.name}: Message from \"{message.sender}\" received! Value = {message.body}")

            value: int = int(message.body)
            self.agent.new_value(value)

    def new_value(self, value: int):
        self.sum += value
        self.count += 1

        average = self.average()
        print(f"{self.name}: New average is {average}\n")

    def average(self):
        return self.sum / self.count

    async def setup(self):
        print(f"{self.name}: Hi!")
        self.add_behaviour(self.HandleIntBehaviour(), int_message_template)

class GeneratorAgent(Agent):
    # Custom agent identifier. For debug purposes only
    name = "Unknown sender"

    # ID of receiver
    to_id = ""

    def __init__(self, jid: str, password: str, name: str, to_id: str):
        super().__init__(jid, password)
        self.name = name
        self.to_id = to_id

    class SendBehaviour(CyclicBehaviour):
        count = 0

        def generate_int_message(self) -> Message:
            value = random.randint(0, 10)

            message = Message(to=self.agent.to_id)
            message.body = str(value)
            message.set_metadata("performative", "inform")
            message.set_metadata("language", "FIFA-SL")
            message.set_metadata("ontology", "INTEGER")
            assert int_message_template.match(message), "Int message should match int template"

            return message

        async def run(self):
            message = self.generate_int_message()

            await self.send(message)
            print(f"{self.agent.name}: Send message \"{message.body}\"")

            if self.count >= 10:
                print(f"{self.agent.name}: Bye!")
                await self.agent.stop()

            await asyncio.sleep(1)
            self.count += 1

    async def setup(self):
        print(f"{self.name}: Hi!")
        self.add_behaviour(self.SendBehaviour())

async def main():
    receiver_agent = AverageNumbersAgent(keys.XMPP_ID_ONE, keys.XMPP_PASSWORD_ONE, "Receiver")
    await receiver_agent.start()

    sender_agent_one = GeneratorAgent(keys.XMPP_ID_TWO, keys.XMPP_PASSWORD_TWO, "Sender 1", keys.XMPP_ID_ONE)
    sender_agent_two = GeneratorAgent(keys.XMPP_ID_THREE, keys.XMPP_PASSWORD_THREE, "Sender 2", keys.XMPP_ID_ONE)

    await sender_agent_one.start()
    await sender_agent_two.start()

    await spade.wait_until_finished(receiver_agent)

if __name__ == "__main__":
    asyncio.run(main())
