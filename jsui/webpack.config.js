const ESLintPlugin = require('eslint-webpack-plugin')
var noop = require('noop-webpack-plugin')
// var isProd = NODE_ENV === 'production'

module.exports = {
    entry: "./src/index.js",
    output: {
      path: __dirname + "/build/js",
      filename: "main.js",
      sourceMapFilename: "[file].map",
      devtoolModuleFilenameTemplate: (info) =>
        `webpack:///${info.absoluteResourcePath.replace(/\\/g, "/")}`,
    },
    devtool: "source-map",
    plugins: [process.env.NODE_ENV ==='lint' ? new ESLintPlugin
      ({
        extensions: ['js', 'jsx'],
        outputReport: true,
        fix: true
      }) : false].filter(Boolean),
    module: {
      rules: [
        {
          test: /\.(js|jsx)$/,
          exclude: /node_modules/,
          use: ["babel-loader"],
        },
        {
          test: /\.svg$/,
          exclude: /node_modules/,
          use: ["svg-inline-loader"],
        },
        {
          test: /\.(png|jpeg|jpg|gif)$/,
          use: [
            {
              loader: "url-loader",
              options: {
                limit: true,
                esModule: false,
              },
            },
          ],
        },
      ],
    },
};
