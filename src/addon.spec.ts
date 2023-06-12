import assert from 'assert/strict';
import { describe, it } from 'node:test';
import { JQRunOptions, JQWrapMode, run } from './addon';

describe('wrap option', () => {
  describe('wrap: auto', () => {
    it('jq . {foo: 1} -> {foo: 1}', () => {
      assert.deepEqual(run('.', { foo: 1 }), { foo: 1 });
    });

    it('jq . [{foo: 1}] -> [{foo: 1}]', () => {
      assert.deepEqual(run('.', [{ foo: 1 }]), [{ foo: 1 }]);
    });

    it('jq .[] [{foo: 1}] -> {foo: 1}', () => {
      assert.deepEqual(run('.[]', [{ foo: 1 }]), { foo: 1 });
    });

    it('jq .[] [{foo: 1}, {bar: 2}] -> [{foo: 1}, {bar: 2}]', () => {
      assert.deepEqual(run('.[]', [{ foo: 1 }, { bar: 2 }]), [{ foo: 1 }, { bar: 2 }]);
    });
  });

  describe('wrap: always', () => {
    const opts: JQRunOptions = { wrap: JQWrapMode.ALWAYS };

    it('jq . {foo: 1} -> [{foo: 1}]', () => {
      assert.deepEqual(run('.', { foo: 1 }, opts), [{ foo: 1 }]);
    });

    it('jq . [{foo: 1}] -> [[{foo: 1}]]', () => {
      assert.deepEqual(run('.', [{ foo: 1 }], opts), [[{ foo: 1 }]]);
    });

    it('jq .[] [{foo: 1}] -> [{foo: 1}]', () => {
      assert.deepEqual(run('.[]', [{ foo: 1 }], opts), [{ foo: 1 }]);
    });

    it('jq .[] [{foo: 1}, {bar: 2}] -> [{foo: 1}, {bar: 2}]', () => {
      assert.deepEqual(run('.[]', [{ foo: 1 }, { bar: 2 }], opts), [{ foo: 1 }, { bar: 2 }]);
    });
  });

  describe('wrap: never', () => {
    const opts: JQRunOptions = { wrap: JQWrapMode.NEVER };

    it('jq . {foo: 1} -> {foo: 1}', () => {
      assert.deepEqual(run('.', { foo: 1 }, opts), { foo: 1 });
    });

    it('jq . [{foo: 1}] -> [{foo: 1}]', () => {
      assert.deepEqual(run('.', [{ foo: 1 }], opts), [{ foo: 1 }]);
    });

    it('jq .[] [{foo: 1}] -> {foo: 1}', () => {
      assert.deepEqual(run('.[]', [{ foo: 1 }], opts), { foo: 1 });
    });

    it('jq .[] [{foo: 1}, {bar: 2}] -> {foo: 1}', () => {
      assert.deepEqual(run('.[]', [{ foo: 1 }, { bar: 2 }], opts), { foo: 1 });
    });
  });
});
