import bindings from 'bindings';

const jq = bindings('node-libjq');

console.debug(jq.run('.', JSON.stringify({ a: 1 })));
