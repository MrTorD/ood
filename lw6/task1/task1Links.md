# Информация о сторонних системах и пользователях
- [Клиент](https://miniflux.app/docs/ui.html)


- [Клиент](https://github.com/miniflux/v2/blob/2.3.5/client/model.go#L18)


- [Администратор](https://github.com/miniflux/v2/blob/2.3.5/internal/api/user_handlers.go#L90)


- [Источник лент](https://miniflux.app/features.html#integrations)


## Факты о системе

- [Daemon запускает Server](https://github.com/miniflux/v2/blob/2.3.5/internal/cli/daemon.go#L42)
- [В качестве БД используется PostgreSQL](https://github.com/miniflux/v2/tree/2.3.5#technical-stuff)
- [Worker занимается работой с БД](https://github.com/miniflux/v2/blob/2.3.5/internal/worker/worker.go#L20)
- [Клиент предоставляет данные, необходимые для входа на ресурсы вледельцев лент](https://github.com/miniflux/v2/blob/2.3.5/client/README.md)
- [Для изменения данных пользователя нужны права администратора](https://github.com/miniflux/v2/blob/2.3.5/internal/api/user_handlers.go#L90)