from spade.template import Template
from spade.agent import Agent
from spade.behaviour import FSMBehaviour, State
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

class ClientAgent(Agent):
    # Visual(in console) agent name. Different from jid
    name = "Client X"

    # How long we wait for message? In seconds
    message_timeout = 10

    # With what service agent work with? JID
    service_id = None

    # Final value from sender. Property created for transfer value from one state to other
    received_value = None

    SEND_REQUEST_STATE = "SEND_REQUEST_STATE"
    WAIT_FOR_REQUEST_RESPONSE = "WAIT_FOR_REQUEST_RESPONSE"
    REFUSED_STATE = "REFUSED_STATE"
    WAIT_FOR_DATA_STATE = "WAIT_FOR_DATA_STATE"
    PROCESS_DATA_STATE = "PROCESS_DATA_STATE"

    def __init__(self, jid: str, password: str, name: str, service_id):
        super().__init__(jid, password)
        self.name = name
        self.service_id = service_id

    class StatesBehaviour(FSMBehaviour):
        async def on_start(self):
            print(f"FSM starting at initial state {self.current_state}")

        async def on_end(self):
            print(f"FSM finished at state {self.current_state}")
            await self.agent.stop()

    class SendRequestState(State):
        async def run(self):
            message = Message(to=self.agent.service_id)
            message.body = "Request data"
            message.set_metadata("language", "FIFA-SL")
            message.set_metadata("performative", "request")
            message.set_metadata("ontology", "REQUEST-DATA")

            assert request_data_template.match(message), "Request message should match request template"
            print(f"{self.agent.name}: Send request message to {message.to}")
            await self.send(message)

            self.set_next_state(self.agent.WAIT_FOR_REQUEST_RESPONSE)

    class WaitForRequestResponseState(State):
        async def run(self):
            print(f"{self.agent.name}: Waiting for request response!")
            message = await self.receive(timeout=self.agent.message_timeout)
            if not message:
                await self.agent.bye()
                return

            print(f"{self.agent.name}: Request response message from \"{message.sender}\" received!", message)
            assert agree_response_template.match(message) or refuse_response_template.match(message), "Data request message should match or agree, of refuse templates"

            if message.metadata["performative"] == "refuse":
                self.set_next_state(self.agent.REFUSED_STATE)
                return

            self.set_next_state(self.agent.WAIT_FOR_DATA_STATE)

    class RefusedState(State):
        async def run(self):
            print(f"\n{self.agent.name}: Request was refused from service!")
            await self.agent.bye()

    class WaitForDataState(State):
        async def run(self):
            print(f"{self.agent.name}: Waiting for data!")

            message = await self.receive(timeout=self.agent.message_timeout)
            if not message:
                await self.agent.bye()
                return

            print(f"{self.agent.name}: Data message from \"{message.sender}\" received! Data = {message.body}")
            assert data_response_template.match(message), "Data message should match data response template"
            self.agent.received_value = message.body
            self.set_next_state(self.agent.PROCESS_DATA_STATE)

    class ProcessDataState(State):
        async def run(self):
            print(f"{self.agent.name}: Final value is {self.agent.received_value}!")
            await self.agent.bye()

    async def bye(self):
        print(f"{self.name}: Bye!")
        await self.stop()

    async def setup(self):
        print(f"{self.name}: Hi!\n")

        fsm = self.StatesBehaviour()
        fsm.add_state(name=self.SEND_REQUEST_STATE, state=self.SendRequestState(), initial=True)
        fsm.add_state(name=self.WAIT_FOR_REQUEST_RESPONSE, state=self.WaitForRequestResponseState())
        fsm.add_state(name=self.REFUSED_STATE, state=self.RefusedState())
        fsm.add_state(name=self.WAIT_FOR_DATA_STATE, state=self.WaitForDataState())
        fsm.add_state(name=self.PROCESS_DATA_STATE, state=self.ProcessDataState())

        fsm.add_transition(source=self.SEND_REQUEST_STATE, dest=self.WAIT_FOR_REQUEST_RESPONSE)
        fsm.add_transition(source=self.WAIT_FOR_REQUEST_RESPONSE, dest=self.REFUSED_STATE)
        fsm.add_transition(source=self.WAIT_FOR_REQUEST_RESPONSE, dest=self.WAIT_FOR_DATA_STATE)
        fsm.add_transition(source=self.WAIT_FOR_DATA_STATE, dest=self.PROCESS_DATA_STATE)

        self.add_behaviour(fsm)
