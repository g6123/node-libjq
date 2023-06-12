import bindings from 'bindings';

const _ = bindings('node-libjq');

export interface JQError extends Error {
  detail: string;
}

export interface JQRunOptions {
  wrap?: JQWrapMode;
}

export function run(program: string, input: unknown, options: JQRunOptions = {}): unknown {
  const results: unknown[] = _.run(program, input);
  return wrap(results, options.wrap ?? JQWrapMode.AUTO);
}

export const enum JQWrapMode {
  AUTO = 'auto',
  ALWAYS = 'always',
  NEVER = 'never',
}

function wrap(data: unknown[], mode: JQWrapMode) {
  switch (mode) {
    case JQWrapMode.AUTO:
      return data.length <= 1 ? data[0] : data;
    case JQWrapMode.ALWAYS:
      return data;
    case JQWrapMode.NEVER:
      return data[0];
  }
}
