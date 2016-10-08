TARGET  := Transform-Feedback
.SUFFIXES:
.SECONDARY:
BUILD   := build
SOURCES := src
CXXFLAGS += -Wall -Wextra -O2 -ggdb3 -std=c++0x
LDFLAGS += -lglut -lGLEW -lGLU -lGL -lpthread
ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT   :=      $(CURDIR)/$(BUILD)/$(TARGET)
export VPATH    :=      $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR  :=      $(CURDIR)/$(BUILD)
CXXFILES        :=      $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cc)))
export OFILES   :=      $(CXXFILES:.cc=.o)
export OUTPUT   :=      $(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT)
else
$(OUTPUT): $(OFILES)
	@echo '[1m[[35mLD[37m][0m' $(notdir $@)
	@g++ $^ -o $@ $(LDFLAGS)
-include $(DEPSDIR)/*.d
endif
%.o: %.cc
	@echo '[1m[[32mCC[37m][0m' $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

