import spade
import asyncio
import keys
from manager_agent import ManagerAgent

async def main():
    graph_agent_ids = [keys.XMPP_ID_TWO, keys.XMPP_ID_THREE]

    manager_agent = ManagerAgent(keys.XMPP_ID_ONE, keys.XMPP_PASSWORD_ONE, "Manager", graph_agent_ids)
    await manager_agent.start()

    await spade.wait_until_finished(manager_agent)
    input("Press any button to continue...")

if __name__ == "__main__":
    asyncio.run(main())
