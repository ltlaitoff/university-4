import spade
import asyncio
import keys
from client_agent import ClientAgent

async def main():
    for i in range(3):
        client_agent = ClientAgent(keys.XMPP_ID_THREE, keys.XMPP_PASSWORD_THREE, f"Client 2.{i}", keys.XMPP_ID_ONE)

        await client_agent.start()
        await spade.wait_until_finished(client_agent)

if __name__ == "__main__":
    asyncio.run(main())
