const path = require('path');
const webpack = require('webpack');
const HtmlWebpackPlugin = require("html-webpack-plugin");
const outputDir = path.join(__dirname, "build/");

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './src/Index.bs.js',
  mode: isProd ? 'production' : 'development',
  devServer: {
    hot: true,
    contentBase: path.resolve(__dirname, 'build'),
    historyApiFallback: true
  },
  output: {
    path: outputDir,
    publicPath: '/',
    filename: '[name].js',
  },
  module: {
    rules: [{
      test: /\.css$/,
      use: [
        "style-loader",
        {
          loader: "css-loader",
          options: {
            importLoaders: 1,
          },
        },
      ],
    }],
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: path.join(__dirname, "src", "index.html"),
    }),
    new webpack.HotModuleReplacementPlugin()
  ]
};
