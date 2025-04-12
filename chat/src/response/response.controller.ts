import {
  Controller,
  Post,
  Body,
  Put,
  Param,
  HttpCode,
  HttpStatus,
} from '@nestjs/common';
import { ResponseService } from './Response.service';
import { CreateResponseDto } from './dto/response.dto';

@Controller('Responses')
export class ResponseController {
  constructor(private readonly ResponseService: ResponseService) {}

  @Post()
  @HttpCode(HttpStatus.CREATED)
  async createResponse(@Body() createResponseDto: CreateResponseDto) {
    return this.ResponseService.createResponse(createResponseDto);
  }
}
