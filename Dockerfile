# 1. Dùng image GCC có sẵn compiler C++
FROM gcc:latest

# 2. Tạo thư mục làm việc trong container
WORKDIR /app

# 3. Copy toàn bộ code + file .txt vào container
COPY . .

# 4. Biên dịch tất cả file .cpp thành 1 file chạy
RUN g++ main.cpp User.cpp Student.cpp Teacher.cpp Database.cpp -o DiemDanhApp

# 5. Chạy chương trình
CMD ["./DiemDanhApp"]
