import spade
import asyncio
import keys
from graph_agent import GraphAgent

async def main():
    graph_agent = GraphAgent(keys.XMPP_ID_THREE, keys.XMPP_PASSWORD_THREE, "Graph 2", keys.XMPP_ID_ONE)
    await graph_agent.start()
    await spade.wait_until_finished(graph_agent)

    input("Press any button to continue...")

if __name__ == "__main__":
    asyncio.run(main())
