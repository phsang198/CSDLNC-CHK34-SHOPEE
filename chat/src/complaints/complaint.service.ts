import {
  BadRequestException,
  Injectable,
  NotFoundException,
} from '@nestjs/common';
import { ArangoDBService } from '../arangodb';
import { CreateComplaintDto } from './dto/create-complaint.dto';
import { UpdateComplaintDto } from './dto/update-complaint.dto';

@Injectable()
export class ComplaintService {
  constructor(private readonly arangoService: ArangoDBService) {}

  async createComplaint(dto: CreateComplaintDto) {
    try {
      const db = this.arangoService.getDatabase();
      const collection = db.collection('complaints');

      if (!dto._from.match(/^[a-zA-Z0-9_-]+\/[a-zA-Z0-9_-]+$/)) {
        throw new BadRequestException(
          'Invalid _from format. Expected <collection>/<key>',
        );
      }
      if (!dto._to.match(/^[a-zA-Z0-9_-]+\/[a-zA-Z0-9_-]+$/)) {
        throw new BadRequestException(
          'Invalid _to format. Expected <collection>/<key>',
        );
      }

      const result = await collection.save(dto);
      return { _id: result._id, ...dto };
    } catch (error) {
      throw new BadRequestException(
        `Failed to create complaint: ${error.message}`,
      );
    }
  }

  async updateComplaint(id: string, dto: UpdateComplaintDto) {
    const db = this.arangoService.getDatabase();
    const collection = db.collection('complaints');

    const exists = await collection.documentExists(id);
    if (!exists) {
      throw new NotFoundException(`Complaint with ID ${id} not found`);
    }

    const updated = await collection.update(id, dto);
    const updatedDocument = await collection.document(id);
    return { _id: updated._id, ...updatedDocument };
  }
}
