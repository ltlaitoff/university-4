import json
from asyncio import sleep
from typing import List

from spade.template import Template
from spade.agent import Agent
from spade.behaviour import CyclicBehaviour, OneShotBehaviour
from spade.message import Message

graph_json_template = Template()
graph_json_template.set_metadata("performative", "request")
graph_json_template.set_metadata("language", "FIFA-SL")
graph_json_template.set_metadata("ontology", "GRAPH-JSON")

result_template = Template()
result_template.set_metadata("performative", "inform")
result_template.set_metadata("language", "FIFA-SL")
result_template.set_metadata("ontology", "RESULT-GRAPH")

class ManagerAgent(Agent):
    # Custom agent identifier. For debug purposes only
    name = "Unknown receiver"
    message_time = 10

    graphs = []

    def __init__(self, jid: str, password: str, name: str, graph_agent_ids: List[str]):
        super().__init__(jid, password)
        self.name = name
        self.create_graphs()
        self.graph_agent_ids = graph_agent_ids

    def create_graphs(self):
        self.graphs = [
            {
                "id": 1,
                "from_node": "A",
                "to_node": "D",
                "graph": {
                    "A": {"B": 1, "D": 4},
                    "B": {"C": 3, "D": 2},
                    "C": {"D": 0},
                    "D": {},
                },
                "expected_length": 3
            },
            {
                "id": 2,
                "from_node": "D",
                "to_node": "T",
                "graph": {
                    "D": {"O": 10},
                    "O": {"T": 3, "P": 2},
                    "T": {"C": 1},
                    "P": {},
                    "C": {},
                },
                "expected_length": 13
            },
            {
                "id": 3,
                "from_node": "A",
                "to_node": "C",
                "graph": {
                    "A": {"B": 1, "C": 4},
                    "B": {"A": 1, "C": 3, "D": 1},
                    "C": {"A": 4, "B": 2, "D": 1},
                    "D": {"B": 5, "C": 1}
                },
                "expected_length": 3
            },
            {
                "id": 4,
                "from_node": "A",
                "to_node": "J",
                "graph": {
                    "A": {"B": 2, "C": 6, "E": 3},
                    "B": {"D": 4, "F": 7},
                    "C": {"B": 1, "F": 5, "G": 2},
                    "D": {"H": 3},
                    "E": {"D": 6, "I": 4},
                    "F": {"H": 2, "J": 5},
                    "G": {"F": 1, "J": 3},
                    "H": {"I": 2},
                    "I": {"J": 6},
                    "J": {"A": 8}
                },
                "expected_length": 11
            },
            {
                "id": 5,
                "from_node": "A",
                "to_node": "F",
                "graph": {
                    "A": {"B": 4, "C": 3, "D": 7},
                    "B": {"E": 2, "F": 6},
                    "C": {"B": 1, "F": 5, "G": 4},
                    "D": {"C": 2, "G": 3},
                    "E": {"H": 3, "I": 7},
                    "F": {"E": 1, "I": 2, "J": 6},
                    "G": {"F": 3, "J": 4},
                    "H": {"I": 2},
                    "I": {"G": 1, "J": 5},
                    "J": {"A": 9}
                },
                "expected_length": 8
            }
        ]

    class SendTasksBehaviour(OneShotBehaviour):
        async def run(self):
            for index, value in enumerate(self.agent.graphs):
                to_id_index = index % len(self.agent.graph_agent_ids)
                to_id = self.agent.graph_agent_ids[to_id_index]

                message = Message(to=to_id)
                message.body = json.dumps(value)
                message.set_metadata("performative", "request")
                message.set_metadata("language", "FIFA-SL")
                message.set_metadata("ontology", "GRAPH-JSON")
                assert graph_json_template.match(message), "Message with graph should match graph-json template"

                await self.send(message)
                print(f"{self.agent.name}: Send graph {value['id']} to agent {to_id}. Expected result: {value['expected_length']}")

    class ReceiveResultBehaviour(CyclicBehaviour):
        async def run(self):
            print(f"{self.agent.name}: Waiting for messages!")
            message = await self.receive(timeout=self.agent.message_time)

            if not message:
                print(f"{self.agent.name}: Bye!")
                await self.agent.stop()
                return

            assert result_template.match(message), "Message with results should match result template"
            print(f"{self.agent.name}: Message from \"{message.sender}\" received! Result = {message.body}")

    async def setup(self):
        print(f"{self.name}: Hi!")

        self.add_behaviour(self.SendTasksBehaviour())
        self.add_behaviour(self.ReceiveResultBehaviour())
