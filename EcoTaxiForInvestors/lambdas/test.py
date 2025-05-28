from reports import lambda_handler as report_handler
from events import lambda_handler as events_handler


class event:
    def __init__(self, get, headers):
        self.get_data = get
        self.headers = headers

    def get(self, key):
        return self.get_data.get(key)


if __name__ == "__main__":
    print("### test report ###")
    print(
        report_handler(
            event({"from": "2024-01-01", "to": "2024-12-31"}, {"password": "meow"}),
            None,
        )
    )
    print("### test events ###")
    print(
        events_handler(
            event(
                {"from": "2024-01-01", "to": "2024-12-31"},
                {"password": "meow"},
            ),
            None,
        )
    )
