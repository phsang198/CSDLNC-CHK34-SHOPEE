import {
  Controller,
  Post,
  Body,
  Put,
  Param,
  HttpCode,
  HttpStatus,
} from '@nestjs/common';
import { ComplaintService } from './complaint.service';
import { CreateComplaintDto } from './dto/create-complaint.dto';
import { UpdateComplaintDto } from './dto/update-complaint.dto';

@Controller('complaints')
export class ComplaintController {
  constructor(private readonly complaintService: ComplaintService) {}

  @Post()
  @HttpCode(HttpStatus.CREATED)
  async createComplaint(@Body() createComplaintDto: CreateComplaintDto) {
    return this.complaintService.createComplaint(createComplaintDto);
  }

  @Put(':id')
  @HttpCode(HttpStatus.OK)
  async updateComplaint(
    @Param('id') id: string,
    @Body() updateComplaintDto: UpdateComplaintDto,
  ) {
    return this.complaintService.updateComplaint(id, updateComplaintDto);
  }
}
