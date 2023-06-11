import bindings from 'bindings';
import { inspect } from 'node:util';

const jq = bindings('node-libjq');

try {
  console.log(inspect(jq.run('entries[]', { a: 2, b: 'c' })));
} catch (error) {
  console.error(inspect(error));
}
