CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -pthread

SRCS = Device.cpp Gonderen.cpp Kullanici.cpp WaveForm.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = waveform_app

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJS)	
	@echo "Bağlayıcı Kullanılıyor $(CXX)"
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	@echo "Derleyici kullanılıyor: $(CXX)"
	$(CXX) $(CXXFLAGS) -c $< -o  $@

INSTALL_DIR = $(CURDIR)/deploy_root

install: $(TARGET)
	@echo "Uygulama Dosyaları $(INSTALL_DIR) dizinine kuruluyor..."
	mkdir -p $(INSTALL_DIR)/usr/bin
	cp $(TARGET) $(INSTALL_DIR)/usr/bin/
	@echo "Kurulum hazırlığı tamamlandı: $(INSTALL_DIR)"

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(INSTALL_DIR) 2>/dev/null || true
