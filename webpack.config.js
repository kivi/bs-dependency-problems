const path = require("path");
const CopyWebpackPlugin = require("copy-webpack-plugin");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const outputDir = path.join(__dirname, "build/");

const isProd = process.env.NODE_ENV === "production";

module.exports = {
  entry: ["./src/Index.bs.js", "./src/styles/styles.js"],
  mode: isProd ? "production" : "development",
  output: {
    path: outputDir,
    filename: "login.js"
  },
  // devtool: 'source-map',
  plugins: [
    new HtmlWebpackPlugin({
      template: "src/index.html",
      inject: false
    })
  ],
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: process.env.PORT || 8040,
    historyApiFallback: true
  },
  module: {
    rules: [
      {
        test: /\.(sa|sc|c)ss$/,
        use: ["style-loader", "css-loader", "sass-loader"]
      }
    ]
  }
};
