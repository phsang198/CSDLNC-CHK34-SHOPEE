package com.mtth.shopee.model;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
public class SearchLogRequest {
    @NotBlank
    private String userId;

    @NotBlank
    private String searchDate;

    @NotBlank
    private String searchQuery;

    @NotBlank
    private String device;

    @NotBlank
    private String location;

    @NotBlank
    private String category;
}