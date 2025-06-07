clc; clear; close all;
% 系统参数
A = [0 1; 0 -2];
B = [0; 1];
C = [1 0];
D = 0;

% 仿真设置
t = 0:0.01:10;
dt = t(2) - t(1); % 步长 0.01
u = ones(size(t)); % 阶跃输入
x0 = [0; 5];       % 初始状态

% 初始化存储
x = zeros(2, length(t));
y = zeros(1, length(t));
x(:,1) = x0;

% 手动递推计算
for k = 1:length(t)-1
    dx = A * x(:,k) + B * u(k);       % 计算导数
    x(:,k+1) = x(:,k) + dx * dt;      % 欧拉法更新状态
    y(k) = C * x(:,k) + D * u(k);     % 计算输出
end
y(end) = C * x(:,end) + D * u(end);   % 最后一个输出

% 可视化结果
figure;
subplot(2,1,1);
plot(t, x(1,:), 'b', t, x(2,:), 'r');
legend('状态 x1', '状态 x2');
title('状态响应');
xlabel('时间 (s)');

subplot(2,1,2);
plot(t, y, 'k');
legend('输出 y');
title('输出响应');
xlabel('时间 (s)');