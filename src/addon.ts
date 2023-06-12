import bindings from 'bindings';

const _ = bindings('node-libjq');

export interface JQError extends Error {
  detail: string;
}

export function run(program: string, input: unknown): unknown {
  return _.run(program, input);
}
