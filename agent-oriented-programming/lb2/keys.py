import os
from dotenv import load_dotenv
load_dotenv()

XMPP_ID_ONE=os.getenv('XMPP_ID_ONE')
XMPP_PASSWORD_ONE=os.getenv('XMPP_PASSWORD_ONE')
XMPP_ID_TWO=os.getenv('XMPP_ID_TWO')
XMPP_PASSWORD_TWO=os.getenv('XMPP_PASSWORD_TWO')
XMPP_ID_THREE=os.getenv('XMPP_ID_THREE')
XMPP_PASSWORD_THREE=os.getenv('XMPP_PASSWORD_THREE')

assert XMPP_ID_ONE is not None and XMPP_ID_ONE != "", "XMPP id ONE environment variable is not defined!"
assert XMPP_PASSWORD_ONE is not None and XMPP_PASSWORD_ONE != "", "XMPP password ONE environment variable is not defined!"
assert XMPP_ID_TWO is not None and XMPP_ID_TWO != "", "XMPP id TWO environment variable is not defined!"
assert XMPP_PASSWORD_TWO is not None and XMPP_PASSWORD_TWO != "", "XMPP password TWO environment variable is not defined!"
assert XMPP_ID_THREE is not None and XMPP_ID_THREE != "", "XMPP id THREE environment variable is not defined!"
assert XMPP_PASSWORD_THREE is not None and XMPP_PASSWORD_THREE != "", "XMPP password THREE environment variable is not defined!"
