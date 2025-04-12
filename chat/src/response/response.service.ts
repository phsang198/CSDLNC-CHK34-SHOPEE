import {
  BadRequestException,
  Injectable,
  NotFoundException,
} from '@nestjs/common';
import { ArangoDBService } from '../arangodb';
import { CreateResponseDto } from './dto/response.dto';

@Injectable()
export class ResponseService {
  constructor(private readonly arangoService: ArangoDBService) {}

  async createResponse(dto: CreateResponseDto) {
    try {
      const db = this.arangoService.getDatabase();
      const collection = db.collection('response');

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
        `Failed to create response ${error.message}`,
      );
    }
  }
}
