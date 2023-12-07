PACKAGE_NAME=tabulate

install:
	@if [ -z "$$(pip show $(PACKAGE_NAME) 2>/dev/null)" ]; then \
		echo "Installing $(PACKAGE_NAME)"; \
		pip install $(PACKAGE_NAME); \
	else \
		echo "$(PACKAGE_NAME) is already installed"; \
	fi

test: install
	python3 mesi.py test

uninstall:
	@if [ -n "$$(pip show $(PACKAGE_NAME) 2>/dev/null)" ]; then \
		echo "Uninstalling $(PACKAGE_NAME)"; \
		pip uninstall -y $(PACKAGE_NAME); \
	else \
		echo "$(PACKAGE_NAME) is not installed"; \
	fi