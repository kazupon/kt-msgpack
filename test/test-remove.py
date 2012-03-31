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
    self._client.call('set', 'remove1', 'hoge')
    ret1 = self._client.call('remove', 'remove1')
    self.assertIsNone(ret1)

    # specific key of no existing record.
    try:
      self._client.call('remove', 'remove2');
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35)

    # specific database name.
    self._client.call('set', 'remove3', 'hoge', { 'DB': 'casket2.kct' });
    ret2 = self._client.call('remove', 'remove3', { 'DB': 'casket2.kct' })
    self.assertIsNone(ret2)

    # not exist database name.
    try:
      self._client.call('remove', 'remove4', { 'DB': 'xxxx' });
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # specific no parameter.
    try:
      self._client.call('remove')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)


if __name__ == '__main__':
  unittest.main()
