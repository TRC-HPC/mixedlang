import logging

pymixedlang_found : bool = True
try:
    import pymixedlang
except:
    pymixedlang_found = False

if pymixedlang_found: # ***************************************************************************

    class MLLogHandler(logging.Handler):
        def __init__(self):
            logging.Handler.__init__(self)
            self.level = logging.DEBUG # lowest level as the parent logger will control the actual level
            self.log_headers = True

        def flush(self):
            pass

        def emit(self, record):
            try:
                msg = self.format(record)
                pymixedlang.log(record.levelname, record.thread, msg);
            except (KeyboardInterrupt, SystemExit):
                raise
            except:
                self.handleError(record)

        def set_headers(self, enable : bool):
            if self.log_headers != enable:
                self.log_headers = enable
                if self.log_headers:
                    self.setFormatter(logging.Formatter('%(pathname)s:%(lineno)d %(funcName)s(): %(message)s'))
                else:
                    self.setFormatter(logging.Formatter('%(message)s'))


    ml_logger = logging.getLogger('mixedlang_demo')
    log_headers = pymixedlang.get_headers()
    if not ml_logger.handlers:
        ml_handler = MLLogHandler()
        ml_handler.log_headers = not log_headers
        ml_logger.addHandler(ml_handler)
    else:
        ml_handler = ml_logger.handlers[0]

    ml_handler.set_headers(log_headers)
    ml_logger.setLevel(pymixedlang.log_level())

else: # not pymixedlang_found *********************************************************************

    logging.basicConfig(format = '%(asctime)s [%(levelname)s][%(thread)d] %(pathname)s:%(lineno)d %(funcName)s(): %(message)s')
    ml_logger = logging.getLogger('mixedlang_demo')
    ml_logger.setLevel(logging.DEBUG)
