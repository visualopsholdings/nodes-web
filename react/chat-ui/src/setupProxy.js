const { createProxyMiddleware } = require('http-proxy-middleware');

module.exports = function(app) {
  app.use('/rest', createProxyMiddleware({ target: 'http://localhost:3000/rest' }));
  app.use('/login', createProxyMiddleware({ target: 'http://localhost:3000/login' }));
  app.use('/logout', createProxyMiddleware({ target: 'http://localhost:3000/logout' }));
};
