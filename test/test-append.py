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

  def test_remove(self):
    that = self

    # normal
    ret1 = self._client.call('append', 'append1', 'hoge')
    self.assertIsNone(ret1)
    self._client.call('append', 'append1', 'hoge')
    ret1 = self._client.call('get', 'append1')
    self._client.assertEqual(ret1, { u'append1': u'hogehoge' })

    # specific database name.
    self._client.call('append', 'append2', 'hoge', { 'DB': 'casket2.kct' });
    ret2 = self._client.call('get', 'append2', { 'DB': 'casket2.kct' })
    self.assertEqual(ret2, { u'append2': u'hoge' })

    # not exist database name.
    try:
      self._client.call('append', 'append3', { 'DB': 'xxxx' });
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # specific expiration time.
    self._client.call('append', 'xt_append', '1', { 'xt': '1000' })
    ret3 = self._client.call('get', 'xt_append')
    self.assertEqual(ret3['value'], '1')
    self.assertTrue(ret3.has_key('xt'))

    # specific no parameter.
    try:
      self._client.call('append')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)


if __name__ == '__main__':
  unittest.main()
