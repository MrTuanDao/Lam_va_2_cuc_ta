# Lam_va_2_cuc_ta
# OS_TinyShell 
# Authors: Đào Quốc Tuấn, Nguyễn Hoàng Lâm, Đào Quốc Tuấn
Research Project 1: Tìm hiểu về Semaphores
Research Project 2: Tìm hiểu về Quản lý bộ nhớ trong vi xử lý intel486 và Hệ điều hành Linux
# Pratice Project 1: 
# Giới thiệu
Thiết kế và cài đặt một shell đơn giản (myShell)
# Mục đích
Nghiên cứu các API quản lý tiến trình trong Windows
Hiểu cách cài đặt và các thức shell làm việc
# Nội dung
Shell nhận lệnh, phân tích và tạo tiến trình con thực hiện
    foreground mode: Shell phải đợi tiến trình kết thúc
    background mode: Shell và tiến trình thực hiện song song
Shell chứa các câu lệnh quản lý tiến trình
    List: in ra DS tiến trình (process Id, Cmd name, status)
    Kill, Stop, Resume.. một background process
    Shell hiểu một số lệnh đặc biệt (exit, help, date, time, dir,..)
    path/addpath : xem và đặt lại biến môi trường
    Shell có thể nhận tín hiệu ngắt từ bàn phím để hủy bỏ
    foreground process đang thực hiện (CRTL+C)
    Shell có thể thực hiện được file *.bat