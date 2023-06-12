# node-libjq

[jq](https://jqlang.github.io/jq/) [C API](https://github.com/jqlang/jq/wiki/C-API:-libjq) binding for Node.js.

## Installation

```sh
yarn add libjq
pnpm add libjq
npm install --save libjq
```

## Example

```javascript
import { run } from 'libjq';

console.log(run('.foo', { foo: 'abc' }));
// abc
```

## Why yet another binding?

[node-jq](https://github.com/sanack/node-jq) spawns new jq process for every function call, which is not optimal in performance and sometimes comes with an unstable result parsing.

This packages uses jq's C API directly, so does not suffer from such problems.
