import init_libnotesalad from '../../build/wasm/libnotesalad/libnotesalad.js';

var libnotesalad;

const getLibNoteSalad = () => {
    return libnotesalad;
}

export const initializeLibNoteSalad = async () => {
    if (!libnotesalad) {
        libnotesalad = await init_libnotesalad();
        libnotesalad.print = (...args) => console.log(...args);
    }
    return libnotesalad;
}

export default getLibNoteSalad;
