module.exports = {
    mode: 'development',
    entry: './src/Worklet/index.js',
    output: {
        filename: 'libnotesalad.js',
    },
    performance: {
        maxEntrypointSize: 1073741824,
        maxAssetSize: 1073741824,
    },
};
