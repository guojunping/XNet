.PHONY: clean All

All:
	@echo ----------Building project:[ XNetFrame - Debug ]----------
	@cd "XNetFrame" && "$(MAKE)" -f "XNetFrame.mk"
clean:
	@echo ----------Cleaning project:[ XNetFrame - Debug ]----------
	@cd "XNetFrame" && "$(MAKE)" -f "XNetFrame.mk" clean
