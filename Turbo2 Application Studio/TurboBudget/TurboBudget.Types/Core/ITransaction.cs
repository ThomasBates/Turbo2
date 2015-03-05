using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TurboBudget.Types
{
    public interface ITransaction
    {
        IAccount FromAccount { get; set; }
        IAccount ToAccount { get; set; }
        string Description { get; set; }
        decimal Amount { get; set; }
    }
}
