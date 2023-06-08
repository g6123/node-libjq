import bindings from 'bindings';

const jq = bindings('node-libjq');

console.debug(jq.calculateSync('.a', JSON.stringify({ a: 1 })));
