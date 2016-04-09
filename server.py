from tornado.web import Application, RequestHandler
from tornado.ioloop import IOLoop
from statsd import StatsClient

statsd_client = StatsClient('localhost', 8125)


class WeightHandler(RequestHandler):
    def get(self):
        statsd_client.gauge('catfi.grams', self.get_argument('grams'))
        self.write('OK')


if __name__ == "__main__":
    application = Application([
        (r"/weight", WeightHandler),
    ])

    application.listen(55669)
    IOLoop.current().start()
