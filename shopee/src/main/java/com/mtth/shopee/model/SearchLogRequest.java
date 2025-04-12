package com.mtth.shopee.model;


import java.time.LocalDate;
import java.util.UUID;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
public class SearchLogRequest {
    @NotBlank
    private UUID userId;

    @NotBlank
    private LocalDate searchDate;

    @NotBlank
    private String searchQuery;

    @NotBlank
    private String device;

    @NotBlank
    private String location;

    @NotBlank
    private String category;
}