import sys
import random
import unittest
import msgpackrpc
from msgpackrpc import error

class TestRemove(unittest.TestCase):

  def setUp(self):
    self._address = msgpackrpc.Address('127.0.0.1', 18801)
    self._client = msgpackrpc.Client(self._address, unpack_encoding='utf-8')

  def tearDown(self):
    self._client.close()

  def test_append(self):
    # normal
    ret1 = self._client.call('append', 'append1', 'hoge')
    self.assertIsNone(ret1)
    self._client.call('append', 'append1', 'hoge')
    ret1 = self._client.call('get', 'append1')
    self.assertEqual(ret1, { u'value': u'hogehoge' })

    # specific database name.
    ret2 = self._client.call('append', 'append2', 'hoge', { 'DB': 'casket2.kct' });
    self.assertIsNone(ret2)
    ret2 = self._client.call('get', 'append2', { 'DB': 'casket2.kct' })
    self.assertEqual(ret2, { u'value': u'hoge' })

    # not exist database name.
    try:
      self._client.call('append', 'append3', 'hoge', { 'DB': 'xxxx' });
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # specific expiration time.
    ret3 = self._client.call('append', 'xt_append', '1', { 'xt': '1000' })
    self.assertIsNone(ret3)
    ret3 = self._client.call_async('get', 'xt_append')
    self.assertEqual(ret3.result.get('value'), u'1')
    self.assertTrue(ret3.result.has_key('xt'))

    # specific no parameter.
    try:
      self._client.call('append')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)


if __name__ == '__main__':
  unittest.main()
