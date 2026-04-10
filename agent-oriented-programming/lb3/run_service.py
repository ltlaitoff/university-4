import spade
import asyncio
import keys
from service_agent import ServiceAgent

async def main():
    service_agent = ServiceAgent(keys.XMPP_ID_ONE, keys.XMPP_PASSWORD_ONE, "Service")
    await service_agent.start()

    await spade.wait_until_finished(service_agent)

if __name__ == "__main__":
    asyncio.run(main())
