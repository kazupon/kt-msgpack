import sys
import random
import unittest
import msgpackrpc
from msgpackrpc import error

class TestSeize(unittest.TestCase):

  def setUp(self):
    self._address = msgpackrpc.Address('127.0.0.1', 18801)
    self._client = msgpackrpc.Client(self._address, unpack_encoding='utf-8')

  def tearDown(self):
    self._client.close()

  def test_seize(self):
    # normal 
    f1 = self._client.call_async('set', 'seize1', 'seize1')
    f1.join()
    ret1 = self._client.call('seize', 'seize1')
    self.assertEqual(ret1, { u'value': u'seize1' })
    try:
      self._client.call('seize', 'seize1')
    except error.RPCError as e:
      self.assertEqual(err.args[0], 35);

    # expiration
    f2 = self._client.call_async('set', 'seize2', 'seize2', { 'xt': '100000' })
    f2.join()
    ret2 = self._client.call('seize', 'seize2')
    self.assertEqual(ret2['value'], u'seize2')
    self.assertTrue(ret2.has_key('xt'))

    # specific key of no existing record.
    try:
      self._client.call('seize', 'xxxx')
    except error.RPCError as e:
      self.assertEqual(err.args[0], 35);

    # specific database name.
    f3 = self._client.call_async('set', 'seize3', '1', { 'DB': 'casket2.kct' })
    f3.join()
    ret3 = self._client.call('seize', 'seize3', { 'DB': 'casket2.kct' })
    self.assertEqual(ret3, { u'value': u'1' })

    # not exist database name.
    try:
      self._client.call('seize', 'xxxx', { 'DB': 'xxxx.kct' })
    except error.RPCError as e:
      self.assertEqual(err.args[0], 34)

    # specific no parameter.
    try:
      self._client.call('seize')
    except error.RPCError as e:
      self.assertEqual(err.args[0], 2)


if __name__ == '__main__':
  unittest.main()
