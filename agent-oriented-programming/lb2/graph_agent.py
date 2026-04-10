import heapq
import json

from spade.agent import Agent
from spade.behaviour import CyclicBehaviour
from spade.message import Message
from spade.template import Template


# Directed graph representation
class Graph:
    # Data saved in format Dict<from_node_id, Dict<to_node_id, weight>>
    graph = {}

    def __init__(self, graph=None):
        if graph is not None:
            self.graph = graph

    def add_edge(self, from_node, to_node: str, weight: float) -> None:
        if from_node not in self.graph:
            self.graph[from_node] = {}
        if to_node not in self.graph:
            self.graph[to_node] = {}

        self.graph[from_node][to_node] = weight

    def find_path(self, from_node: str, to_node: str):
        distances = {}
        for node in self.graph:
            distances[node] = float("inf")

        heap_queue = [(0, from_node)]

        while heap_queue:
            distance, node = heapq.heappop(heap_queue)

            if distance > distances[node]:
                continue

            for neighbor, weight in self.graph[node].items():
                neighbor_distance = distance + weight

                if neighbor_distance >= distances[neighbor]:
                    continue

                distances[neighbor] = neighbor_distance
                heapq.heappush(heap_queue, (neighbor_distance, neighbor))

        return distances[to_node]

result_template = Template()
result_template.set_metadata("performative", "inform")
result_template.set_metadata("language", "FIFA-SL")
result_template.set_metadata("ontology", "RESULT-GRAPH")

graph_json_template = Template()
graph_json_template.set_metadata("performative", "request")
graph_json_template.set_metadata("language", "FIFA-SL")
graph_json_template.set_metadata("ontology", "GRAPH-JSON")

class GraphAgent(Agent):
    # Custom agent identifier. For debug purposes only
    name = "Unknown sender"
    message_time = 10

    # ID of receiver
    manager_id = ""

    def __init__(self, jid: str, password: str, name: str, to_id: str):
        super().__init__(jid, password)
        self.name = name
        self.manager_id = to_id

    class MessagesBehaviour(CyclicBehaviour):
        async def run(self):
            print(f"{self.agent.name}: Waiting for messages!")
            message = await self.receive(timeout=self.agent.message_time)

            if not message:
                print(f"{self.agent.name}: Bye!")
                await self.agent.stop()
                return

            assert graph_json_template.match(message), "Message with graph should match graph-json template"
            print(f"{self.agent.name}: Message from \"{message.sender}\" received! Value = {message.body}")

            parsed_message = json.loads(message.body)
            graph = Graph(parsed_message["graph"])
            result = graph.find_path(parsed_message["from_node"], parsed_message["to_node"])

            print(f"{self.agent.name}: Result is = {result}")

            response_value = json.dumps({"id": parsed_message['id'], "result": result})

            message = Message(to=self.agent.manager_id)
            message.body = response_value
            message.set_metadata("performative", "inform")
            message.set_metadata("language", "FIFA-SL")
            message.set_metadata("ontology", "RESULT-GRAPH")
            assert result_template.match(message), "Result message should match result template"

            await self.send(message)
            print(f"{self.agent.name}: Send message \"{message.body}\"")

    async def setup(self):
        print(f"{self.name}: Hi!")
        self.add_behaviour(self.MessagesBehaviour())
