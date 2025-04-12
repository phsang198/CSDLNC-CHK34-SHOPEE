package com.mtth.shopee;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

@SpringBootApplication
@EnableScheduling
public class SpringBootProjectApplication {
    public static void main(String[] args) {
        System.out.println("✅ Application is starting...");
        SpringApplication.run(SpringBootProjectApplication.class, args);
        System.out.println("✅ Application started!");
    }
}