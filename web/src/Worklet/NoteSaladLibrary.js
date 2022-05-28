import init_libnotesalad from '../../build/wasm/libnotesalad/libnotesalad.js';

const libnotesalad = init_libnotesalad();
libnotesalad.print = (...args) => console.log(...args);

export default libnotesalad;
